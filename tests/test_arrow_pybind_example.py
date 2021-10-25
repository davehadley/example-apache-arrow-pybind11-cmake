import tempfile

def test_package_can_be_imported():
    import arrow_pybind_example
    assert arrow_pybind_example.example_load_csv

def test_load_csv_works():
    import arrow_pybind_example
    with tempfile.TemporaryFile() as tmpfile:
        csvcontent = "a,1,2.0\nb,3,4.0"
        tempfile.write(csvcontent)
        table = arrow_pybind_example.example_load_csv()
        assert table
