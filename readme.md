GLUS 
====

A pure C scene setup and render library.

You can try it by this [release](./doc/glus.zip)

![demo image](./doc/demo.png)

# quick start
You can steup a scene simply by this command <br>
`camera		1 2 1		0 0 0	0 1 0`<br>
`projection	55 1.5	.1 100`<br>
`fps 30`<br>
`shadelevel 1`<br>
`shape grid`<br>
`shape axis`<br>
and you can get scene like bellow.It has a grid and axis,and the shape will be draw a mesh.<br>
![quick start image](./doc/quick.png)

# control the object
You custom the object after create it.
## basic command
You can change the color or material of shape by `diffuse`,`ambient`,`specular`,`shininess` and `transp`.

You can transform a shape by `translate`,`scale`,`rotate`.

For example,you custom the grid and axis by this
``` 
shape grid		diffuse	.2 .2 .2	specular 0 0 0 ambient 0 0 0
shape axis		scale	9	9	9
```
So you will get this view:<br>
![basic image](./doc/basic.png)