# gr-bar

This repository includes custom GRC blocks made for the implementation of a physical layer wireless sensor network testbed built on the work of Bastian Bloessl. This testbed is expected to measure BER, SER, PER, Latency, Throughput, and SNR of a communication system.

## Getting Started

### Prerequisites

Aside from standard GNU Radio blocks, you need to have the following GRC repositories installed:

```
https://github.com/bastibl/gr-ieee802-15-4
https://github.com/bastibl/gr-foo

```
Also, this was built using ADALM-PLUTO so you may also need to install this:
```
https://github.com/analogdevicesinc/gr-iio

```

### Installing

Installing this is by the usual cmake installation

```
mkdir build
cd build
cmake ../
sudo make install
sudo ldconfig
```

Some GRC files in the 'examples' folder may need to have their python files generated to complete some blocks.

## Usage

This testbed is actually an undergraduate project and is fully documented in [this](https://drive.google.com/file/d/1k-PZ24ZglsvscZG_fhPUXv4Ov0nlif5m/view?usp=sharing) document.

## Authors

* **Herlan Kester R. Benitez** (herlan.benitez@eee.upd.edu.ph)
* **Christian Haven M. Cabuso** (christian.haven.cabuso@eee.upd.edu.ph)


## Acknowledgments

We would like to thank our advisers:
* Anastacia Alvarez (anastacia.alvarez@eee.upd.edu.ph)
* Maria Theresa De Leon (theresa.de.leon@eee.upd.edu.ph)
* John Richard Hizon (richard.hizon@eee.upd.edu.ph)
* Marc Rosales (marc.rosales@eee.upd.edu.ph)
* Christopher Santos (christopher.santos@eee.upd.edu.ph)

and our examiner:
* Paul Jason Co (paul.co@eee.upd.edu.ph)

For their insights and guidance in this project.

