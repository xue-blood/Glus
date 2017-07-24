GLUS 
====

A pure C scene setup and render library.

You can try it by this [release](./doc/glus.zip)

![demo image](./doc/demo.png)

## quick start
You can steup a scene simply by this command <br>
`camera		1 2 1		0 0 0	0 1 0`<br>
`projection	55 1.5	.1 100`<br>
`fps 30`<br>
`shadelevel 1`<br>
`shape grid`<br>
`shape axis`<br>
and you can get scene like bellow.It has a grid and axis,and the shape will be draw a mesh.<br>
![quick start image](./doc/quick.png)

## basic

### control the object
You custom the object after create it.

You can change the color or material of shape by `diffuse`,`ambient`,`specular`,`shininess` and `transp`.

You can transform a shape by `translate`,`scale`,`rotate`.

For example,you custom the grid and axis by this
``` 
shape grid		diffuse	.2 .2 .2	specular 0 0 0 ambient 0 0 0
shape axis		scale	9	9	9
```
So you will get this view:<br>
![basic image](./doc/basic.png)



### light
Light is enable by default,and if you not setup a light, a point white light will be create at `(20.0f, 12.0f, 3.0f)`.

You can disable light use `islight 0` ,to enable it use `islight 1`.

To setup a light,use `light		0	12 8.5 2 1   .9 .8 .5 1`.The light type is `GL_LIGHT0` and it's a point light at `(12, 8.5, 2)` with color `(.9, .8, .5, 1)`.To setup a parallel light change `12 8.5 2 1` to `12 8.5 2 0`.


### camera
To display a scene correctly, you need setup camera and projection,for example
    `camera	10 20 10	0 0 0	0 1 0`<br>
    This will create a camera at point `(10,20,10)` watch to point `(0,0,0)` and it's up vector is `(0,1,0)`.<br>
    Also command `projection	55 1.5	.1 100` will setup a perspect project matrix *angle* is *55* ,rato is 1.5f,near plane is at 0.1f and far plane is at 100.0f.

### shape
There are some simple shape build in c code, to use it just type `shape shape_name`.
Shape name contain `"axis", "sphere", "cube", "teapot", "grid", "snow", "square", "cylinder", "cone"`.
eg.The amaze snow shape:

![snow shape image](./doc/snow.png)

For create more shape ,you can see [mesh](#mesh),[peano curve](#peano) and the [chaos game](#chaos)

### mesh


### peano

### chaos

### 

## advance 