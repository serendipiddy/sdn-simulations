# sdn-simulations

Some notes on the process can be seen in [this presentation](https://docs.google.com/presentation/d/1ElT0QQqeX9JS7-AH0TakTibIBCBwGZOnlxhNdhSr6Jc/edit?usp=sharing)

To compile the simulation, you'll need access to the omnetpp sample queueinglib.
Run the following command:

```
cd gotos_model/
make -f makemakefiles
cd src
make
cd ..
out/gcc-release/src/gotos_model -f src/omnetpp.ini -l ../queueinglib/libqueueinglib.so
```
