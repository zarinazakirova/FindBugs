# L08P02 Find Bugs 2 
**Build the container image**

 To build the Infer docker image use the following commands: 


1. In the terminal, change directory to the FindBugs directory. Replace /path/to/FindBugs with the path to FindBugs directory.

```
cd /path/to/FindBugs
```
2. Now build the container image.

```
docker build -t infer .
```

**Start the container**

Now that you have an image, you can run the application in a container. To do so, use the docker run command.

```
docker run -it --rm -v $PWD/src:/FindBugs2 infer /bin/bash
```

Then type: 
```
infer -- clang -c exercise.c
```
**Update the code**

If have updated the code, you do not need to rebuild the docker image. Just type again: 

```
infer -- clang -c exercise.c
```

**Remove the container**

If you want to stop a container, you can do it by using ``exit`` command. Note that in this case the container would also be removed. 