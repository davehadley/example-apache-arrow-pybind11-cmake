import tempfile
from pathlib import Path


def test_package_can_be_imported():
    import arrow_pybind_example

    assert arrow_pybind_example.example_load_csv


def test_load_csv_works():
    import arrow_pybind_example

    with tempfile.TemporaryDirectory() as directory:
        filepath = Path(directory) / "example.csv"
        with open(filepath, "wt") as tmp:
            csvcontent = "a,1,2.0\nb,3,4.0"
            tmp.write(csvcontent)
        table = arrow_pybind_example.example_load_csv(str(filepath))
        assert table
