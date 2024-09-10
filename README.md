**Ender Compute** is a project about running long-running processes such as tests in the background.

It is inspired by Jenkins.


# The project
The code is compiled using BetterThanBatch. You can download a release instead of compiling the project from scratch.

## Products and software
The project develops two software products.
1. A backend for managing and computing jobs.
2. A frontend UI (web server) for the backend. It allows users to create see jobs.
3. (maybe) A command line tool when you don't have a web browser (or like terminals more).

# File structure
Most directories are self explanatory (tests, examples, docs).

`public` is for web code (html, js) it is accesible by clients that connect to the web server.

`src` is code for the backend and command line tool.

# Background
Where does the name from?
- **Ender** - The End in Minecraft, a void far away, can't be reached my normal means
- **Compute** - Computation, processing
- **Ender Compute** - Computation on a server somewhere
