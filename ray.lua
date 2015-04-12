white = rgb.new(1, 1, 1)
gray = rgb.new(0.6, 0.6, 0.6)

dom = 0.85
sub = 0.4

whiteWall = material.new()
whiteWall.Color = rgb.new(dom, dom, dom)
whiteWall.Specular = 0
whiteWall.Diffuse = 1
whiteWall.GlossyReflective = 1
whiteWall.Reflective = 1
whiteWall.Refractive = 1
whiteWall.RefrAbsorbance = 1

grayWall = material.new()
grayWall.Color = gray
grayWall.Specular = 0
grayWall.Diffuse = 1
grayWall.GlossyReflective = 1
grayWall.Reflective = 1
grayWall.Refractive = 1
grayWall.RefrAbsorbance = 1

blueWall = material.new()
blueWall.Color = rgb.new(sub, sub, dom)
blueWall.Specular = 0
blueWall.Diffuse = 1
blueWall.GlossyReflective = 1
blueWall.Reflective = 1
blueWall.Refractive = 1
blueWall.RefrAbsorbance = 1

redWall = material.new()
redWall.Color = rgb.new(dom, sub, sub)
redWall.Specular = 0
redWall.Diffuse = 1
redWall.GlossyReflective = 1
redWall.Reflective = 1
redWall.Refractive = 1
redWall.RefrAbsorbance = 1

mat = material.new()
mat.Color            = rgb.new(1, 0, 0)
mat.Specular         = 0.8
mat.Diffuse          = 1.0
mat.GlossyReflective = 0.0
mat.Reflective       = 1.0
mat.Refractive       = 1.0
mat.RefrAbsorbance   = 1.0

scene.add("Sphere", whiteWall, 160, 0, 163, 0)
scene.add("Sphere", whiteWall, 160, 0, -163, 0)
scene.add("Sphere", whiteWall, 160, 0, 0, -170)
scene.add("Sphere", whiteWall, 160, 0, 0, 163)
scene.add("Sphere", blueWall, 160, -163, 0, 0)
scene.add("Sphere", redWall, 160, 163, 0, 0)

scene.add("Sphere", mat, 0.8, 1.0, -2.2, -2.0)

lightmat = material.new()
lightmat.Color    = white
lightmat.Emissive = 3.0

scene.add("Sphere", lightmat, 1.0, 0.0, 1.9, 1.0)

t = transform.translate(vector.new(0, -1, -8))

dim = 512
scene.set("Camera", t, dim, dim, dim)

scene.set("LightSamples", 1)
scene.set("GlossyReflectiveSamples", 1)
scene.set("Depth", 1)
scene.set("Samples", 1)
scene.set("PathEnableDirectLighting", true)
scene.set("PathEnableIndirectIllum", true)

