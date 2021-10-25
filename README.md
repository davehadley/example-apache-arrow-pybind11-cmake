# Apache Arrow C++ Exensions with Python Bindings built with CMake

An example package that demonstrates:
    - C++ extensions to Apache Arrow built with CMake
    - python bindings to those exensions with pybind11.
    - dependencies installed with conda

Similar to <https://github.com/vaexio/vaex-arrow-ext>, however in this example the C++ component builds with CMake.

## Usage

Create and activate the conda environment:

```bash
conda env create -f environment.yml \
&& conda activate example-apache-arrow-pybind11-cmake
```

Install the python package:
```
pip install -e .
```

Test that the build was successful:
```
pytest tests
```

## License

This example uses the pybind CMake example which is licensed under <https://github.com/pybind/cmake_example> a BSD-like license.