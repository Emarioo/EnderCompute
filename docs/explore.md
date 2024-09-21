This document contains my thoughts when planning out this project. They are rather scattered. Other documents are better structured.

# Prototype
We'll begin with a prototype. What should it be?

- A web server where user can interact with jobs.
- The backend part of the server which runs and manages jobs.

## Design
The web interface will have these things:
- Feature to create, edit, and remove jobs.
- Running any kind of process.
- Supplying process with arguments and files.
- Seeing list of jobs, that are running, will be run, have run.
- Information about who started a job, multiple users may use the system.
- Information about when it was scheduled, when it began running and when it finished.
- Ability to see the output of finished and running jobs.

To run a job you must be authenticated. For simplicity you have to enter the correct key. There are no users, usernames, passwords. Everyone uses the same key. We can renew the key every monday.

The key is stored in local storage. This is safe as long as we don't compromised third-party scripts or cross-site scripts.

To run a test you need:
- Create a job.
- Create a script template and assign it to the job.
- Start the job.

A script template is a script that the backend will execute. It is a template because many jobs can use the same templates. You don't want to write the same script for every job.

## Backend details
There is the web server and the job system. The web server authenticates the HTTP requests and calls the corresponding API method of the job system based on the url (path and queries).

The job system has a list of **jobs**, **scripts**, and **job console outputs**.

If user sends a "Start job" request then we:
- Search for job in the list
- Search for script
- Create a directory with a valid name
- Create script in directory (scripts are stored as strings)
- Run script in directory

The script can be any kind. It can clone repo and run tests, clone repo and run git requests.

Ideally the scripts would run in a virtual machine for safety. Or maybe you have the EnderCompute main server and
some job servers that are unrelated.

When testing BTB we can do this:
```bash
git clone <url>
cd <path>
python3 build.py
bin\btb.exe --test
```

## Authenticate
- The user inserts a key in the web browser.
- Whenever user sends requests the key is safely included.
- Server checks if key is valid.
- If valid, send requested data
- If not, send "You are not authenticated"

# Version 0.1.0

**Nice but not essential features for now**
- Ability to pause jobs that are running, delay jobs that will run.
- Ability to categorize jobs based on failure and success.

- Command line interface
- Rebuild jobs
- Filters and search bar for jobs

# Backend
The backend is two things.
- The data on disc and in memory that stores jobs.
- The system that deals with the data.

# The code module
The code exists in a struct representing the job system.

The job system has internal functionality and API methods like a database.

You start, stop, create, and list jobs using these methods.

## The methods
- Create, edit, remove
- Running a job git, bash, python, btb? and supplying job with arguments, options, files.
- Get information about job, how it's running and stuff...
- Get output of job.

## When and how do jobs run
User creates job. Then click run on the job. This is sent from client: `/api/start?id=13`.

Server sees this, gets the job and starts running it.

**What is a job?**
- Tests (tests in virtual machines, different platforms)
- Release builds

**How do we run tests**
When testing BTB we can do this:
```bash
git clone <url>
cd <path>
python3 build.py
bin\btb.exe --test
```

## Authentication


# Other stuff

**Why did I choose BTB language instead of C++?**

Because I like it and I need to test it on a standalone project.
Performance in the backend code will not be a problem.
The job managing code will do little computation, it's the jobs themselves that take time so
the choice of language is purely based on preference.

Also, BTB can be compiled on Linux and Windows with ease. I can't say the same about C++.