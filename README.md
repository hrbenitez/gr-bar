# gr-bar
This repository includes custom GRC blocks made for the implementation of a physical layer wireless sensor network testbed built on the work of Bastian Bloessl.

Using cmake will install 3 new blocks to GRC: the Output Synchronizer, the Packet Checker, and Signal-Noise Cutter.

# gr-bar

This repository includes custom GRC blocks made for the implementation of a physical layer wireless sensor network testbed built on the work of Bastian Bloessl. This testbed is expected to measure BER, SER, PER, Latency, Throughput, and SNR of a communication system.

## Getting Started

It is assumed that the user is already familiar with GRC and the installation of additional repositories for GRC.

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

This testbed is actually an undergraduate project and is fully documented in this (link to follow) document.

## Authors

* **Herlan Kester R. Benitez** - *Initial work*
* **Christian Haven M. Cabuso** - *Initial work*

## Acknowledgments

We would like to thank our advisers:
* Anastacia Alvarez
* Maria Theresa De Leon
* John Richard Hizon 
* Marc Rosales
* Christopher Santos
and our examiner:
* Paul Jason Co

For their insights and guidance in this project.

