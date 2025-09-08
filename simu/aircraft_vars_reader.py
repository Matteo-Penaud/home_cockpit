from pathlib import Path

import yaml


def load_yaml_file(yaml_file_path: Path) -> dict:
    """Load a configuration YAML file.

    Args:
        yaml_file_path (Path): _description_

    Returns:
        dict: _description_

    """
    with Path.open(yaml_file_path) as stream:
        return yaml.safe_load(stream)


def get_vars(loaded_yaml: dict) -> dict:
    return loaded_yaml["vars"]
