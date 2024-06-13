from os import getenv, listdir, path
import os
from setuptools import setup, Extension
from Cython.Build import cythonize
from dotenv import load_dotenv

# TODO: ADD ASSIMP DEPENDENCY

load_dotenv()

MODULE_NAME = "renderer"

C_PATH = path.join(path.dirname(__file__), "src/")

INCLUDE_DIRS = [
    getenv("INCLUDE_DIR"),
    path.join(path.dirname(__file__), "glad/include/"),
]
LIBRARY_DIRS = [
    getenv("STATIC_LIB_DIR")
]

LIBRARIES = [f.replace(".lib", "") for f in listdir(getenv("STATIC_LIB_DIR")) if f.endswith(".lib")]

c_deps = [
    *[path.join(C_PATH, cppf) for cppf in listdir(C_PATH) if cppf.endswith(".cpp")],
    path.join(path.dirname(__file__), "glad/src/gl.c")
]

EXTENSIONS = [
    Extension(MODULE_NAME,
              sources=[path.join(path.dirname(__file__), MODULE_NAME + ".pyx"), *c_deps],
              language="c++",
              include_dirs=INCLUDE_DIRS,
              libraries=LIBRARIES + [
                  "kernel32", "user32", "gdi32", "winmm", "imm32", "ole32",
                  "oleaut32", "version", "uuid", "advapi32", "setupapi",
                  "shell32", "dinput8", "opengl32"
              ],
              library_dirs=LIBRARY_DIRS,
              extra_compile_args=["/MT", "/std:c++20", "/MP", "/Ox"],
              extra_link_args=[]
              )
]


setup(
    name=MODULE_NAME,
    ext_modules = cythonize(EXTENSIONS,
        compiler_directives={
            "language_level": 3,
            "profile": False
        }
    ),
)