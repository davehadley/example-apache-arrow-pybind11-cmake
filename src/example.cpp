
#include <arrow/csv/api.h>
#include <arrow/io/api.h>
#include <arrow/ipc/api.h>
#include <arrow/pretty_print.h>
#include <arrow/python/pyarrow.h>
#include <arrow/result.h>
#include <arrow/status.h>
#include <arrow/table.h>
#include <pybind11/pybind11.h>

#include <iostream>
#include <string>

using arrow::Status;

namespace {

std::shared_ptr<arrow::Table> loadCsvFileToArrowTable(
    std::string csv_filename) {
  auto input_file =
      arrow::io::ReadableFile::Open(csv_filename.c_str()).ValueOrDie();
  auto csv_reader =
      arrow::csv::TableReader::Make(arrow::io::default_io_context(), input_file,
                                    arrow::csv::ReadOptions::Defaults(),
                                    arrow::csv::ParseOptions::Defaults(),
                                    arrow::csv::ConvertOptions::Defaults())
          .ValueOrDie();
  auto table = csv_reader->Read().ValueOrDie();
  return std::move(table);
}

PyObject* pythonLoadCsvFileToArrowTable(std::string csv_filename) {
  arrow::py::import_pyarrow();
  return arrow::py::wrap_table(loadCsvFileToArrowTable(csv_filename));
}

Status RunMain(int argc, char** argv) {
  std::string csv_filename = "test.csv";
  std::string arrow_filename = "test.arrow";

  std::cerr << "* Reading CSV file '" << csv_filename << "' into table"
            << std::endl;

  auto table = loadCsvFileToArrowTable(csv_filename);

  std::cerr << "* Read table:" << std::endl;
  ARROW_RETURN_NOT_OK(arrow::PrettyPrint(*table, {}, &std::cerr));

  std::cerr << "* Writing table into Arrow IPC file '" << arrow_filename.c_str()
            << "'" << std::endl;
  ARROW_ASSIGN_OR_RAISE(auto output_file, arrow::io::FileOutputStream::Open(
                                              arrow_filename.c_str()));
  ARROW_ASSIGN_OR_RAISE(auto batch_writer, arrow::ipc::MakeFileWriter(
                                               output_file, table->schema()));
  ARROW_RETURN_NOT_OK(batch_writer->WriteTable(*table));
  ARROW_RETURN_NOT_OK(batch_writer->Close());

  return Status::OK();
}

}  // namespace

int main(int argc, char** argv) {
  Status st = RunMain(argc, argv);
  if (!st.ok()) {
    std::cerr << st << std::endl;
    return 1;
  }
  return 0;
}

namespace py = pybind11;

PYBIND11_MODULE(arrow_pybind_example, m) {
  m.def("example_load_csv", &pythonLoadCsvFileToArrowTable,
        R"pbdoc(
        Loads a CSV file.
    )pbdoc");
  m.attr("__version__") = "dev";
}