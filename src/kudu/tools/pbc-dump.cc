// Copyright (c) 2015, Cloudera, inc.
// Confidential Cloudera Information: Covered by NDA.

#include <iostream>
#include <string>

#include <gflags/gflags.h>
#include <glog/logging.h>

#include "kudu/gutil/gscoped_ptr.h"
#include "kudu/util/env.h"
#include "kudu/util/logging.h"
#include "kudu/util/pb_util.h"
#include "kudu/util/status.h"

using kudu::Status;
using std::cerr;
using std::endl;
using std::string;

namespace kudu {
namespace pb_util {

Status DumpPBContainerFile(const string& filename) {
  Env* env = Env::Default();
  gscoped_ptr<RandomAccessFile> reader;
  RETURN_NOT_OK(env->NewRandomAccessFile(filename, &reader));
  ReadablePBContainerFile pb_reader(reader.Pass());
  RETURN_NOT_OK(pb_reader.Init());
  RETURN_NOT_OK(pb_reader.Dump(&std::cout));

  return Status::OK();
}

} // namespace pb_util
} // namespace kudu

int main(int argc, char **argv) {
  google::ParseCommandLineFlags(&argc, &argv, true);
  kudu::InitGoogleLoggingSafe(argv[0]);
  if (argc != 2) {
    cerr << "usage: " << argv[0] << " <protobuf container filename>" << endl;
    return 2;
  }

  Status s = kudu::pb_util::DumpPBContainerFile(argv[1]);
  if (s.ok()) {
    return 0;
  } else {
    cerr << s.ToString() << endl;
    return 1;
  }
}
