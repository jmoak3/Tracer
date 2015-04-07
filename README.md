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
    

* PathTracer
  * Importance Distribution Reflecting across surfaces
  * Per-Pixel Monte Carlo AA Sampling
  * High Speed
    * Fast Convergence for less samples, using Direct Illumination, Diffuse Inter-reflectance, and Russian Roulette
    * KDTrees for Triangle Meshes
  
  
Imgur album of renders showing its progress:
http://imgur.com/a/e5kgZ
