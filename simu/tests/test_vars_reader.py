from pathlib import Path

from .. import aircraft_vars_reader

SCRIPT_PATH = Path(__file__).parent
TESTFILE_OK_PATH = SCRIPT_PATH / "testfile_ok.yaml"


def test_load_file_ok() -> None:
    assert aircraft_vars_reader.load_yaml_file(TESTFILE_OK_PATH)


def test_read_vars() -> None:
    loaded_yaml = aircraft_vars_reader.load_yaml_file(TESTFILE_OK_PATH)
    assert aircraft_vars_reader.get_vars(loaded_yaml).keys() == {"VAR_1:1", "VAR_2"}


# def test_read_vars_
