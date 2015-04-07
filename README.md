# Tracer
A "simple" C++ RayTracer

The goals are to produce high quality images fast using interesting techniques.

Started out as a lightning fast Raytracer (phong/diffuse/shadow samples and all that), but now has pathtracer renderer alongside it. 

It's pretty cool, but hopefully it'll be cooler soon.

Features:
---------

* RayTracer
  * Direct Illumination
    * Specular, Diffuse, and Glossy Reflections supported
    * Per-Point Monte Carlo Light Sampling
    * Per-Point Monte Carlo Shadow Sampling
    * Per-Pixel Monte Carlo AA Sampling
    * Per-Point Monte Carlo Glossy Reflection Sampling
  * Refraction
    * Uses refration and reflection rays to produce final image
  * Optimized
    * KDTrees for Triangle Meshes
    * Early outs help avoid unnecessary recursion
    * Some CPU optimizations
    * Fast Bounding Box Intersections
    

* PathTracer
  * Gorgeous sample-convergence based images
  * Russian Roulette Ray elimination
  * Per-Pixel Monte Carlo AA Sampling
  * High Speed
    * Fast Convergence for Few Samples
      * Direct Illumination
        * Diffuse, Specular, and Shadow Tests
        * Diffuse Inter-reflectance to approximate Global Illumination
        * Importance Distribution Reflections Across Surfaces
    * KDTrees for Triangle Meshes
    * Some CPU optimizations
    * Early outs help avoid unnecessary recursion
    * Fast Bounding Box Intersections
  
  
Imgur album of renders showing its progress:
http://imgur.com/a/e5kgZ
