# cephgeorep
Ceph File System Remote Sync Daemon for Ceph Mimic.  
For use with a Ceph Mimic file server to trickle files to a remote backup server, i.e. georeplicate your Ceph fs.

## Prerequisites
You must have a Ceph file system and rsync installed. You must also set up passwordless SSH from your sender (local) to your
receiver (remote backup) with a public/private key pair to allow rsync to send your files.

## Build instructions
Build with `g++ -std=c++11 cephfssyncd.cpp -o cephfssyncd`

## Configuration
Default config file generated by daemon: (/etc/ceph/cephfssyncd.conf)

```
SND_SYNC_DIR=                               (path to directory you want backed up)
RECV_SYNC_HOST=                             (ip address/hostname of your backup server)
RECV_SYNC_DIR=                              (path to backup directory)
LAST_RCTIME_DIR=/var/lib/ceph/cephfssync/   (path to where the last modification time is stored)
SYNC_FREQ=10                                (frequency to check directory for changes, in seconds)
IGNORE_HIDDEN=false                         (change to true to ignore files and folders starting with '.')
RCTIME_PROP_DELAY=5000                      (delay between detecting change and taking snapshot*)
COPMRESSION=false                           (compresses files before sending for slow network)
LOG_LEVEL=1                                 (log output verbosity)
# 0 = minimum logging
# 1 = basic logging
# 2 = debug logging
```

\* The Ceph file system has a propagation delay for recursive ctime to make its way from the changed file to the
top level directory it's contained in. To account for this delay in deep directory trees, there is a user-defined
delay to ensure no files are missed.
```
Small directory tree    (0-50 total dirs):        RCTIME_PROP_DELAY=1000
Medium directory tree   (51-500 total dirs):      RCTIME_PROP_DELAY=2000
Large directory tree    (500-10,000 total dirs):  RCTIME_PROP_DELAY=5000
Massive directory tree  (10,000+ total dirs):     RCTIME_PROP_DELAY=10000
```
The 10,000 ms delay passed a test with 137,256 directories and 1,000 files with 100% sync rate.

## Usage

To install this daemon, move build output to /usr/bin/ as cephfssyncd. Place cephfssyncd.service in /etc/systemd/system/. Launch
daemon by running `systemctl start cephfssyncd`, and run `systemctl enable cephfssyncd` to launch the daemon at start up. To monitor output of daemon, run `watch -n 1 systemctl status cephfssyncd`.

## Notes
If your backup server is down, cephfssyncd will try to launch rsync and fail, however it will retry the sync at 30 second
intervals. All new files in the server created while cephfssyncd is waiting for rsync to succeed will be synced on the next interval.

```
   ___________________  
 /o  _       ______   o\\   _____       _
|   | |  _  |  ___/     || |  __ \     (_)               
|   | |_| | | |___      || | |  | |_ __ ___   _____  ___  
|   |___  | |___  |     || | |  | | '__| \ \ / / _ \/ __|
|       | |  ___| |     || | |__| | |  | |\ V /  __/\__ \
|       |_| /_____/     || |_____/|_|  |_| \_/ \___||___/
 \o___________________o// 
