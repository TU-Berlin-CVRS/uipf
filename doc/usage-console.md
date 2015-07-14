# Console Interface

The console interface provides two different use cases:

- run an existing processing chain by specifying its file name:

        ./uipf -c example.yaml
      
- run a single module:

        ./uipf <moduleName> -i <input> [-p <param> ...] [-o <output>]
      
  Examples:

  ```sh
  # run the gaussian module with parameter sigmaX=5 on the input.jpg image
  # the output will be the default name "input_result.jpg"
  ./uipf gaussian -i input.jpg -p sigmaX:5
      
  # same as above but now the result is stored in "notdefault.png"
  ./uipf gaussian -i input.jpg -p sigmaX:5 -o notdefault.png
      
  # an example with two named inputs, one is the image and one is the kernel:
  # the same syntax applies when there are multiple outputs
  ./uipf convolution -i image:input.jpg -i kernel:kernel.png -p sigmaX:5 -o out.png
  ```
