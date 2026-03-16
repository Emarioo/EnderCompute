Prototype document for **Ender Compute**


# General

**Ender Compute** is a job runner and scheduler with web interface.
Mainly meant for C/C++/Odin-like projects.

## Web interface

Start jobs, see log files from them.

SSH into the build directory of the jobs to view things in detail.
Also lets you rebuild and rerun it.

Each job is named, `BARF #1`, `BARF #2`, `BASIN #56`...

You can delete jobs. If you accidently deleted a job it will be kept for 48 hours and then deleted.
See TRASH for deleted jobs.

Each job runs a script (python, shell, executable).
There are no environment variables. If you need any, the script specifies them.

You can have job templates.



Jobs have descriptions.
When searching for jobs it will also search descriptions.
Descriptions usually contain version number and dependencies.



