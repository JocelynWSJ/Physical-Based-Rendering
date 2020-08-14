Path Tracer Part III: Multiple Importance Sampling and Microfacet Materials
======================

**University of Pennsylvania, CIS 561: Advanced Computer Graphics, Homework 6**

Shijia's result
------------
**Name: Shijia Wang;  PennKey: wangshj9**

**PT_cornellBoxTwoLights.png**

Full Lighting Integrator(Power Heuristic)

![](./CornellP.png)

Full Lighting Integrator(Balance Heuristic)

![](./CornellB.png)

Direct Lighting Integrator

![](./CornellDirect.png)

Naive Integrator

![](./CornelNaive.png)


**PT_gradientMirrorBox.png**

Full Lighting Integrator

![](./GradientP.png)

Direct Lighting Integrator

![](./GradientDirect.png)

Naive Integrator

![](./GradientNaive.png)


**PT_roughMirrorBoxMap.png**

Full Lighting Integrator

![](./MapP.png)

Direct Lighting Integrator

![](./MapDirect.png)

Naive Integrator

![](./MapNaive.png)


**PT_roughMirrorBoxUniform.png**

Full Lighting Integrator

![](./UniformP.png)

Full Lighting Integrator(Balance Heuristic)

![](./UniformB.png)

Direct Lighting Integrator

![](./UniformDirect.png)

Naive Integrator

![](./UniformNaive.png)


**PT_veachScene.png**

Full Lighting Integrator

![](./VeachP.png)

Direct Lighting Integrator

![](./VeachDirect.png)

Naive Integrator

![](./VeachNaive.png)

Overview
------------
You will implement an update to your `DirectLightingIntegrator` class
that improves the way it samples the direct illumination on more complex
surfaces such as the new microfacet materials we've provided.

We have provided you with updated and new files for your existing path tracer
base code. Please copy your existing files into the `assignment_package` folder,
but __DO NOT__ replace any of the files we have provided with this assignment.
For some of your files, you may have to add function implementations to
various files to make your project compile again. However, just to re-iterate,
it is vital that you __DO NOT__ replace any of the files we have given you
for this assignment.

Useful Reading
---------
Once again, you will find the textbook will be very helpful when implementing
this homework assignment. We recommend referring to the following chapters:
* 8.4: Microfacet Models
* 9.2: Material Interface and Implementations
* 13.10: Importance Sampling

The Light Transport Equation
--------------
#### L<sub>o</sub>(p, &#969;<sub>o</sub>) = L<sub>e</sub>(p, &#969;<sub>o</sub>) + &#8747;<sub><sub>S</sub></sub> f(p, &#969;<sub>o</sub>, &#969;<sub>i</sub>) L<sub>i</sub>(p, &#969;<sub>i</sub>) V(p', p) |dot(&#969;<sub>i</sub>, N)| _d_&#969;<sub>i</sub>

* __L<sub>o</sub>__ is the light that exits point _p_ along ray &#969;<sub>o</sub>.
* __L<sub>e</sub>__ is the light inherently emitted by the surface at point _p_
along ray &#969;<sub>o</sub>.
* __&#8747;<sub><sub>S</sub></sub>__ is the integral over the sphere of ray
directions from which light can reach point _p_. &#969;<sub>o</sub> and
&#969;<sub>i</sub> are within this domain.
* __f__ is the Bidirectional Scattering Distribution Function of the material at
point _p_, which evaluates the proportion of energy received from
&#969;<sub>i</sub> at point _p_ that is reflected along &#969;<sub>o</sub>.
* __L<sub>i</sub>__ is the light energy that reaches point _p_ from the ray
&#969;<sub>i</sub>. This is the recursive term of the LTE.
* __V__ is a simple visibility test that determines if the surface point _p_' from
which &#969;<sub>i</sub> originates is visible to _p_. It returns 1 if there is
no obstruction, and 0 is there is something between _p_ and _p_'. This is really
only included in the LTE when one generates &#969;<sub>i</sub> by randomly
choosing a point of origin in the scene rather than generating a ray and finding
its intersection with the scene.
* The __absolute-value dot product__ term accounts for Lambert's Law of Cosines.

Updating this README (5 points)
-------------
Make sure that you fill out this `README.md` file with your name and PennKey,
along with your test renders. You should render each of the new scenes we have
provided you, once with each integrator type. At minimum we expect renders using
the default sample count and recursion depth, but you are encouraged to try
rendering scenes with more samples to get nicer looking results.

`MicrofacetBRDF` Class 
-------------
We have provided an implementation of the MicrofaceBRDF class for you. If you want
to see how these materials interact with light, try rendering
`PT_roughMirrorBoxUniform.json` using your `NaiveIntegrator` with
100 samples per pixel and a recursion depth of 5:

![](roughMirror100SamplesNaive.png)

`DiffuseAreaLight::Pdf_Li` (5 points)
---------
Implement the `Pdf_Li` function declared in `diffusearealight.h` so that it
returns the PDF of the light's `Shape` given an `Intersection` from which the
light is being viewed and a viewing direction `wi`.

`DirectLightingIntegrator` Update (80 points)
-----------
One of the main components of this homework assignment is to write an integrator
that incorporates __multiple importance sampling__ to more effectively estimate
the direct illumination within a scene containing light sources of different
sizes and materials of different glossiness. In `DirectLightingIntegrator::Li`,
you will now generate __two__ direct lighting sample rays. One ray will be the
same as before: a ray that travels directly to a point on the chosen light
source. The other light sample ray will generated using the BSDF sampling
technique used in your `NaiveIntegrator`, except it will only return light color
if it intersects the light source you chose to sample. You will then weight each
of these energy samples according to the Power Heuristic and average them
together to produce an overall sample color. Note that the BSDF-sampled ray is
__not__ a recursive ray as it was in the `NaiveIntegrator`, but just another
direct illumination sampling ray.

Balance Heuristic and Power Heuristic (10 points)
----------
At the bottom of `directlightingintegrator.cpp`, you will find definitions of
two functions: `BalanceHeuristic` and `PowerHeuristic`. Implement both of these
functions so they can be used in your implementation of
`DirectLightingIntegrator::Li`. For your final renders, we ask that you provide
at least one example of a scene rendered with the balance heuristic and one
example of the same scene rendered with the power heuristic. For best render
results, we recommend you use the power heuristic for the rest of your images.

MIS Example Renders
--------
Each of the images below was produced using multiple importance sampling and 100
samples per pixel, unless otherwise noted.

`PT_cornellBoxTwoLights.json`

![](twoLightsMISDirect.png)

`PT_roughMirrorBoxUniform.json`

![](roughmirror100SamplesMISDirect.png)

Extra credit (30 points maximum)
-----------
In addition to the features listed below, you may choose to implement __any
feature you can think of__ as extra credit, provided you propose the idea to the
course staff through Piazza first.

__You must provide renders of your implemented features to receive credit for
them.__

#### `MicrofacetBTDF` Class (10 points)
Implement a BTDF that uses a microfacet distribution to generate its sample
rays. You will also have to add a new `Material` class or modify an existing
one, e.g. `TransmissiveMaterial`, to support this BxDF. You will also have to
add code to the `JSONLoader` class in order to load and render a scene that uses
this new material. Refer to how we implemented the updated `MirrorMaterial` for
ideas.

#### Additional `MicrofacetDistribution` Classes (5 points each)
Implement additional microfacet distribution classes for use in your
`MicrofacetBRDF` class, such as a Blinn distribution, a Phong distribution, or a
 Beckmann distribution.

#### Custom scene (5 points)
Now that you have implemented several different BSDFs and have written an
efficient global illumination integrator, you should try to create a customized
JSON scene of your own to render. We encourage everyone to be creative so you
all have unique material for your demo reels. Note that a custom scene that
includes one of the extra credit features listed above is a requirement of
those features implicitly; you must create an entirely novel scene (i.e. not
a variation on the Cornell box) to receive this extra credit.

Submitting your project
--------------
Along with your project code, make sure that you fill out this `README.md` file
with your name and PennKey, along with your test renders.

Rather than uploading a zip file to Canvas, you will simply submit a link to
the committed version of your code you wish us to grade. If you click on the
__Commits__ tab of your repository on Github, you will be brought to a list of
commits you've made. Simply click on the one you wish for us to grade, then copy
and paste the URL of the page into the Canvas submission form.
