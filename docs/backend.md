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