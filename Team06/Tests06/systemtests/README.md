# System Testing

### *Folder Layout*

Each folder contains a different program and has the following files:

- `run.bat`: For Windows Users, to run the system tests
- `run.sh`: For Mac Users, to run the system tests
- `source.txt`: The source program we are testing against
- `sourceNumbered.txt`: A well formatted `source.txt` to be used for reference only, 
with proper indentation and line numbering 
- And various test files based on relationships

### *Running a system test*

E.g: To run `systemtest1`,

1. Open a terminal in the `systemtest1` folder or `cd` into it.
2. For Windows Users, run: 

    `.\run.bat`

    For Mac Users, run:
    
    `./run.sh`

    `.out` files should appear.

3. To view the results of the test, run the following command in the same folder:
(Having two terminals would help)
    
    `python -m http.server {PORT_OF_YOUR_CHOICE}`

4. Visit `localhost:{PORT_OF_YOUR_CHOICE}` to view the test results.

### *Todo*
- Make it more optimized
- Test on Mac
