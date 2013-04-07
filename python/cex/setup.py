from distutils.core import setup, Extension

module1 = Extension('cex', sources = ['cex.c'])

setup (name = 'cex',
       version = '0.1',
       description = 'Example',
       ext_modules = [module1])
