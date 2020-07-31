import os
import socket
import sys
import tempfile
import threading

import pymonetdb

try:
    from MonetDBtesting import process
except ImportError:
    import process


# Find a free network port
def freeport():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.bind(('', 0))
    port = sock.getsockname()[1]
    sock.close()
    return port


with tempfile.TemporaryDirectory() as farm_dir:
    os.mkdir(os.path.join(farm_dir, 'node1'))
    os.mkdir(os.path.join(farm_dir, 'node2'))

    node1_port = freeport()
    with process.server(mapiport=node1_port, dbname='node1',
                        dbfarm=os.path.join(farm_dir, 'node1'),
                        stdin=process.PIPE, stdout=process.PIPE,
                        stderr=process.PIPE) as node1_proc:
        node1_conn = pymonetdb.connect(database='node1', port=node1_port, autocommit=True)
        node1_cur = node1_conn.cursor()

        node1_cur.execute("create table remote_data (id int, name varchar(2048))")
        node1_cur.execute("insert into remote_data values (1, 'Name 1')")
        node1_cur.execute("select * from remote_data")
        print(node1_cur.fetchall())
        node1_cur.execute("select * from remote_data where name like 'N%'")
        print(node1_cur.fetchall())

        node2_port = freeport()
        with process.server(mapiport=node2_port, dbname='node2',
                            dbfarm=os.path.join(farm_dir, 'node2'),
                            stdin=process.PIPE, stdout=process.PIPE,
                            stderr=process.PIPE) as node2_proc:
            node2_conn = pymonetdb.connect(database='node2', port=node2_port, autocommit=True)
            node2_cur = node2_conn.cursor()

            node2_cur.execute("create remote table remote_data (id int, name varchar(2048)) on 'mapi:monetdb://localhost:{}/node1/sys/remote_data'".format(node1_port))
            node2_cur.execute("select * from remote_data")
            print(node2_cur.fetchall())
            node2_cur.execute("select * from remote_data where name like 'N%'")
            print(node2_cur.fetchall())
            node2_cur.execute("select rank() over () from remote_data where name like 'N%'")
            print(node2_cur.fetchall())
            node2_cur.execute("select name like 'N%' from remote_data")
            print(node2_cur.fetchall())
            node2_cur.execute("select corr(1,1) from remote_data")
            print(node2_cur.fetchall())
            node2_cur.execute("select corr(1,1) over () from remote_data")
            print(node2_cur.fetchall())
            node2_cur.execute("select count(*) over (), max(name) over (), min(name) over (partition by name order by name rows between 3 preceding and 2 preceding) from remote_data")
            print(node2_cur.fetchall())

            # cleanup: shutdown the monetdb servers and remove tempdir
            out, err = node1_proc.communicate()
            sys.stderr.write(err)

            out, err = node2_proc.communicate()
            sys.stderr.write(err)