#include <arrow/csv/api.h>
#include <arrow/io/api.h>
#include <arrow/python/pyarrow.h>
#include <arrow/table.h>
#include <pybind11/pybind11.h>

#include <string>

namespace py = pybind11;

namespace {

std::shared_ptr<arrow::Table> load_table_from_csv(std::string csv_filename) {
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

py::object example_load_csv(std::string csv_filename) {
  arrow::py::import_pyarrow();
  PyObject* object = arrow::py::wrap_table(load_table_from_csv(csv_filename));
  return py::reinterpret_steal<py::object>(object);
}

}  // namespace

PYBIND11_MODULE(arrow_pybind_example, m) {
  m.def("example_load_csv", &example_load_csv,
        R"pbdoc(
        Loads a CSV file as a PyArrow table.
    )pbdoc");
  m.attr("__version__") = "0.1.0";
}