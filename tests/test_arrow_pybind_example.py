import tempfile
from pathlib import Path


def test_package_can_be_imported():
    import arrow_pybind_example

    assert arrow_pybind_example.example_load_csv


def test_package_version():
    import arrow_pybind_example

    assert arrow_pybind_example.__version__ == "0.1.0"


def test_load_csv_works():
    import arrow_pybind_example

    with tempfile.TemporaryDirectory() as directory:
        filepath = Path(directory) / "example.csv"
        with open(filepath, "wt") as tmp:
            csvcontent = "str,int,float\na,1,2.0\nb,3,4.0\nc,5,6.0\n"
            tmp.write(csvcontent)
        table = arrow_pybind_example.example_load_csv(str(filepath))
        assert table.to_pydict() == {
            "str": ["a", "b", "c"],
            "int": [1, 3, 5],
            "float": [2.0, 4.0, 6.0],
        }
