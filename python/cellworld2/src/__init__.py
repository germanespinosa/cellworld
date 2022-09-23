from .core import *
import os
import requests

cellworld_data_base_uri = "https://raw.githubusercontent.com/germanespinosa/cellworld_data/master/"
cellworld_cache_folder = "."


def get_resource(resource_type: str, key0, *argv) -> str:
    resource_uri = cellworld_data_base_uri + resource_type + "/" + key0
    file_path = ""
    if cellworld_cache_folder:
        file_path = cellworld_cache_folder + "/" + resource_type + "/" + key0
    for arg in argv:
        resource_uri += "." + arg
        if cellworld_cache_folder:
            file_path += "." + arg
    if cellworld_cache_folder and os.path.exists(file_path):
        content = ""
        with open(file_path, "r") as text_file:
            content = text_file.read()
        return content
    else:
        response = requests.get(resource_uri)
        return response.text
