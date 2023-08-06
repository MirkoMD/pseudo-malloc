# Pseudo Malloc
Project for the [*Operating Systems*](https://sites.google.com/diag.uniroma1.it/sistemi-operativi-2022-23) course of Computer Science Engineering at Sapienza University of Rome, academic year 2022/2023

[comment]: <> (TODO: image with sth)

## Table of contents
- [Introduction](#introduction)
- [Technologies](#technologies)
- [Acknowledgements](#acknowledgements)
- [License](#license)

## Introduction

- A lib `malloc` replacement

The system relies on mmap for the physical allocation of memory, but handles the requests in
2 ways:
1. for small requests (< 1/4 of the page size) it uses a buddy allocator.
    Clearly, such a buddy allocator can manage at most page-size bytes. For simplicity it uses a single buddy allocator, implemented with a bitmap
    that manages 1 MB of memory for these small allocations.

2. for large request (>=1/4 of the page size) it uses a `mmap`.


## Technologies
- C

## Acknowledgements
GitLab repo on which the project is based:
  - [Sistemi Operativi 2022 23](https://gitlab.com/grisetti/sistemi_operativi_2022_23/) 
    - Grisetti Giorgio, De Rebotti Lorenzo, Brizi Leonardo

## License
[MIT](https://choosealicense.com/licenses/mit/)
    
