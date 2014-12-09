//
//  Shader.vsh
//  ExampleGL
//
//  Created by Daniel Jalkut on 1/6/12.
//  Copyright (c) 2012 Red Sweater Software. All rights reserved.
//
 
attribute vec4 position;
varying vec4 positionInput;
 
void main()
{
  positionInput = position;
    gl_Position = position;
}