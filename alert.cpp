/*
    Copyright (C) 2019 Joshua Boudreau
    
    This file is part of cephgeorep.

    cephgeorep is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    cephgeorep is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with cephgeorep.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "alert.hpp"
#include "config.hpp"
#include "rctime.hpp"
#include <iostream>
#include <boost/system/error_code.hpp>

boost::system::error_code ec;

std::string errors[NUM_ERRS] = {
  "Error opening config file.",
  "Error creating path.",
  "Error reading ceph.dir.rctime attribute of directory. Are you using CEPH?",
  "Error reading mtime attribute of file.",
  "Error reading number of files or directories in subdirectory.",
  "Error removing snapshot directory.",
  "Error forking process.",
  "Error launching specified sync program.",
  "Error while waiting for rsync to exit.",
  "Specified sync program is not installed on this system.",
  "Encountered unkown error while launching specified sync program.",
  "SND_SYNC_DIR does not exist: no such directory.",
  "Remote user does not have permission to write to backup directory."
};

void error(int err, boost::system::error_code ec_){
  std::cerr << "ERROR: " << errors[err] << std::endl;
  writeLast_rctime(last_rctime);
  exit(ec_.value());
}

void Log(std::string msg, int lvl){
  if(config.log_level >= lvl) std::cout << msg << std::endl;
}
