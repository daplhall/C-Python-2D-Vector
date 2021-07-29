from distutils.core import setup, Extension

module = Extension('Vector2d',
					sources=['./src/Vector2d.c'],
					#extra_compile_args = ["-O3",
					#					  "-funroll-loops",
					#					  "-ftree-vectorize",
					#					  "-fopt-info-vec-optimized",
					#					  "-std=c++14"],

					) #modules to build.

# extra_compile_args = ["-O3", 
# 										  "-Wall", 
# 										  "-pedantic", 
# 										  "-msse4.1", 
# 										  "-ftree-vectorize", 
# 										  "-ffast-math"],
setup(name = 'Vector2d', 
    version='1.0',
    description = 'sda',
    ext_modules = [module])