# L08P02 Find Bugs 2 
### To build Infer docker image use this commands: 
```
docker build -t infer .
```

```
docker run -it --rm -v $PWD/src:/FindBugs2 infer /bin/bash
```

```
infer -- clang -c exercise.c
```
