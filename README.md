# Tracer
A "simple" C++ RayTracer made to learn more about graphics.

The goals are to produce high quality images fast using interesting techniques.

Started out as a lightning fast Raytracer (phong/diffuse/shadow, refraction, aa samples etc), but now has pathtracer renderer alongside it. 

It's pretty cool, and hopefully it'll be cooler soon.

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
  * Optimized
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
    * Options For Fast Convergence for Few Samples
      * Direct Illumination
        * Diffuse, Specular, and Shadow Tests
      * Diffuse Inter-reflectance to approximate Global Illumination
      * Importance Distribution Reflections Across Surfaces
    * KDTrees for Triangle Meshes
    * Some CPU optimizations
    * Early outs help avoid unnecessary recursion
    * Fast Bounding Box Intersections

Quality object pasted to renderer object of your choice (path, ray) that describes how many samples of each type to take, which light approx techniques (Direct Illumination + Diffuse Inter-reflectancy vs doing a bajillion bouncing rays) to use to speed up, etc.
  
Imgur album of renders showing its progress:
http://imgur.com/a/e5kgZ
