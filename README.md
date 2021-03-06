# Tracer
A "simple" C++ RayTracer made to learn more about graphics, with a simple Lua interface by Github user stephanwilliams.

The goals are to produce high quality images fast using interesting techniques.  
Started out as a lightning fast Raytracer (phong/diffuse/shadow, refraction, aa samples etc), but now has pathtracer renderer alongside it.  
It's pretty cool, and hopefully it'll be cooler soon.


The Lua interface currently only supports spheres - just comment out the code that branches to Lua if you want to render triangles with this!

Images: 
![ScreenShot](http://i.imgur.com/QGuxkGy.png)  
Path Tracer, no fast convergence techniques, pre-optimization - 300 samples, 1000x1000 ~ 4 Hours

![ScreenShot](http://i.imgur.com/DhQFTTH.png)  
Path Tracer, MultiCore Direct Illumination and Diffuse Inter-reflectance - 100 samples, 512x512 ~ 47 Seconds (46608 MS)

![ScreenShot](http://i.imgur.com/92z2vF9.png)  
Path Tracer, Pre-optimizations, Direct Illumination and Diffuse Inter-reflectance - 100 samples, 512x512 ~ 8 Hours (29935139 MS)

![ScreenShot](http://i.imgur.com/9HevjW9.png)  
Ray Tracer pre-optimizations - 1000x1000 ~ Somewhere less than 20 Minutes

![ScreenShot](http://i.imgur.com/OcpYo2K.png)  
Ray Tracer, pre-KDTree and optimizations, faking GI using low sample Glossy Reflections - 1000x1000 ~ 5 Hours 


Features:
----------
* RayTracer
  * Triangle Mesh and Primitive Shape Support
  * Direct Illumination
    * Specular, Diffuse, and Glossy Reflections supported
    * Per-Point Monte Carlo Light Sampling
    * Per-Point Monte Carlo Shadow Sampling
    * Per-Pixel Monte Carlo AA Sampling
    * Per-Point Monte Carlo Glossy Reflection Sampling
  * Refraction
  * Translucency
  * High Speed
    * Multithreading Support
    * KDTrees for Triangle Meshes
    * Early outs help avoid unnecessary recursion
    * Some CPU optimizations
    * Fast Bounding Box Intersections
    

* PathTracer
  * Pretty sample-convergence based images
  * Russian Roulette Ray elimination
  * Per-Pixel Monte Carlo AA Sampling
  * Triangle Mesh and Primitive Shape Support
  * High Speed
    * Multithreading Support
    * Options For Fast Convergence for Few Samples
      * Direct Illumination
        * Diffuse, Specular, and Shadow Tests
      * Diffuse Inter-reflectance to approximate Global Illumination
      * Importance Distribution Reflections Across Surfaces
    * KDTrees for Triangle Meshes
    * Some CPU optimizations
    * Early outs help avoid unnecessary recursion
    * Fast Bounding Box Intersections

Quality object passed to renderer object of your choice (path, ray) that describes how many samples of each type to take, which light approx techniques (Direct Illumination + Diffuse Inter-reflectance vs doing a bajillion bouncing rays) to use to speed up, etc.


My Computer
-----------
* MSI GT70 Laptop
* Intel Core™ i7-3610QM
* NVIDIA GeForce GTX 670M | 3GB GDDR5
* Windows 8.1
* 12 GB DDR3 RAM

  
Imgur album of renders showing its progress: http://imgur.com/a/e5kgZ
-----------------------------------------------------------------------
