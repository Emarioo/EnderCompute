# Project
Management related

- Make a prototype
    - Web interface
    - Backend
    - Command line interface?
    - Security risk assessment (SRA)
- Plan version 0.1.0 (maybe this is the prototype)
- More thorough SRA

# Interface
- Plan the web UI
- Plan the command line interface
- Create the web interface

# Backend
- Plan the system
- How to run jobs
- Authentication, authorization
- HTTPS and OpenSSL

# Security risk assessment
All information gathered should be put into writing.
- List all assets (files, access to things, code, keys, passwords, sensitive information) and grade by how bad it would be if a malicious actor got access.
- List all attack vectors of the whole project. (input from user, TCP connection, VPS, file system...?)
- List all common web vulnerabilites.
- Investigate how the software prevents the vulnerabilities and whether the assets are safe.
- Make a plan for how to fix and maintain security. When fixing vulnerabilites, write down a procedure which successfully utilizes the exploit. Once the exploit is fixed, the procedure should not longer work. When it comes to maintaining security, look at which parts of the code require extra care when modifying to avoid introducing vulnerabilites. Preferably add indiscreet comments in those parts.

Then we fix the exploits but that's not part of the assessment itself.

# Logging
Useful but not strictly necessary. I mostly want to experiment with creating good logs.

**Requirements**
- Tracing requests or events using unique ids. If one log message says "GET request of /path from IP" then an ID should be attached to this request. Every log that is related to it like "Responded with OK", "Not authorized" should contain the related ID.
- Levels FATAL, ERROR, WARN, INFO, DEBUG. fatal is when program is crashing, error is when a problem occured on the server like a file is missing for no reason but the server can keep going without it, warn is when user did something bad (suspicious or illformed input), info is everything except debug such as start of request, creation of files, execution of scripts, creation of threads, sub-processes. Debug is logs you use in development. It makes it easy to skip everything else and just focus on that.
- Convenient methods for logging. Logging can be a hazzle and if it is a developer will skip logging. So logging should be easy and not repetitive. For example, you can bind information to each log so you don't have to add it every time.
```c++
// bad example
logger.info("ID: "+request_id+", New request from {IP} to {URL}")
logger.info("ID: "+request_id+", Respond with OK")

// much better, you can abstract how the log message is formatted if you decide to change in the future too.
// In the above code you would need to rewrite lots of log messages.
logger.bind(INFO, "ID: "+request_id)
logger.info("New request from {IP} to {URL}")
logger.info("Respond with OK")
logger.unbind(...)
```
- Searchability is very important. You should be able to search the logs and they should be categorized properly too. Authentication and HTTP stuff can go into one log file. The job system and script runners into another. It should be possible to filter logs by time. You can use grep or a custom tool to filter and search.
- Logs should obviously contain useful information such as these: request id, time/date (timestamp?), IP, requested route, authorized or not, file IO, process/thread creation, what scripts are running (jobs), startup and shutdown of server.

## Resources
[**Ideals for Logging on the Backend** by *Ikechi Michael*](https://mykeels.medium.com/ideals-for-logging-on-the-backend-494fb6296c2f)