# ECGR4181 Final Project

# Noteworthy Files
 - ./report.pdf                            - The report
 - report/First Draft Presentation.pptx    - The presentation

## Directory Structure
 - report/    -   Files for the report and presentation
 - project/   -   Files for the trace file and filter scripts
 - src/       -   Source code for the simulator
 - results/   -   Files with the raw results of the simulation


## To Compile
 - Create bin/ folder
 - Create dep/ folder
 - run Makefile (`make`)
 - run the simulation (`bin/main.out <args>`)

## Args Example
`bin/main.out -fproject/rsync_actual_final.trace -pLocal --shiftreg-bits=12 --sig-bits=12 --sig-lco-bits=7`