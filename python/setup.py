from setuptools import setup, find_packages
from setuptools.command.build_ext import build_ext
from setuptools.command.install import install
import os
import sys
import shutil


if sys.platform.startswith("win"):
    shared_library_name = 'DSICE.dll'
else:
    shared_library_name = 'libDSICE.so'


class CustomBuildExt(build_ext):
    def run(self):
        build_dir = os.path.join(os.path.dirname(__file__), '..', 'build', 'src')
        shared_library = os.path.join(build_dir, shared_library_name)
        package_dir = os.path.join(os.path.dirname(__file__), 'dsice')

        if not os.path.exists(shared_library):
            raise FileNotFoundError(f"Shared library not found at {shared_library}")

        target_path = os.path.join(package_dir, shared_library_name)
        shutil.copy2(shared_library, target_path)
        print(f"Copied {shared_library} to {target_path}")
        super().run()


class CustomInstall(install):
    def run(self):
        install.run(self)
        package_install_dir = os.path.join(self.install_lib, 'dsice')
        shared_library = os.path.join(os.path.dirname(__file__), 'dsice', shared_library_name)
        target_path = os.path.join(package_install_dir, shared_library_name)

        shutil.copy2(shared_library, target_path)
        print(f"Copied {shared_library} to {target_path}")


setup(
    name='dsice',
    version='1.0.0',
    packages=find_packages(),
    package_data={'dsice': [shared_library_name]},
    include_package_data=True,
    cmdclass={
        'build_ext': CustomBuildExt,
        'install': CustomInstall,
    }
)
