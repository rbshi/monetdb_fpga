from __future__ import print_function

try:
    from MonetDBtesting import process
except ImportError:
    import process
import os, sys, socket, time

dbfarm = os.getenv('GDK_DBFARM')
tstdb = os.getenv('TSTDB')

if not tstdb or not dbfarm:
    print('No TSTDB or GDK_DBFARM in environment')
    sys.exit(1)

def freeport():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.bind(('', 0))
    port = sock.getsockname()[1]
    sock.close()
    return port

cloneport = freeport()

dbname = tstdb
dbnameclone = tstdb + 'clone'

#master = process.server(dbname = dbname, stdin = process.PIPE, stdout = process.PIPE, stderr = process.PIPE)
slave = process.server(dbname = dbnameclone, mapiport = cloneport, stdin = process.PIPE, stdout = process.PIPE, stderr = process.PIPE)

c = process.client('sql', server = slave, stdin = process.PIPE, stdout = process.PIPE, stderr = process.PIPE)

# make sure we are a little behind the wlc70.py
time.sleep(3);
# the previous test did not prepare the log files
cout, cerr = c.communicate('''\
call wlr.master('%s');
#select now();
call wlr.replicate(now());
select * from tmp70;
''' % dbname)

sout, serr = slave.communicate()
#mout, merr = master.communicate()

#sys.stdout.write(mout)
sys.stdout.write(sout)
sys.stdout.write(cout)
#sys.stderr.write(merr)
sys.stderr.write(serr)
sys.stderr.write(cerr)

def listfiles(path):
    sys.stdout.write("#LISTING OF THE WLR LOG FILE\n")
    for f in sorted(os.listdir(path)):
        if f.find('wlr') >= 0:
            file = path + os.path.sep + f
            sys.stdout.write('#' + file + "\n")
            try:
                x = open(file)
                s = x.read()
                lines = s.split('\n')
                for l in lines:
                    sys.stdout.write('#' + l + '\n')
                x.close()
            except IOError:
                sys.stderr.write('Failure to read file ' + file + '\n')

#listfiles(os.path.join(dbfarm, tstdb))
#listfiles(os.path.join(dbfarm, tstdb, 'wlc_logs'))
listfiles(os.path.join(dbfarm, tstdb + 'clone'))
