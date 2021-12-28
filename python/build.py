#!/bin/python3
from easy_pack import EasyPackModule
from os import path

module = EasyPackModule.read('.')
if not path.exists('setup/setup.py') or path.getctime('__info__.py') > path.getctime('setup/setup.py'):
	print('package info file has changed, rebuilding setup')
	module.create_setup_files('../setup')
if module.build_module('python-build'):
	print('build succeded')
	print('use twine upload --repository-url [pypi-repository-url] dist/* to upload the package')
	module.save('.')
else:
	print('build failed')