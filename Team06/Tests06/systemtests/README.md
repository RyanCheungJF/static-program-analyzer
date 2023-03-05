# System Testing

### _Folder Layout_

Each folder contains a different program and has the following files:

- `serve.bat/sh`: Starts up all the servers needed
- `run.bat`: For Windows Users, to run the system tests
- `run.sh`: For Mac Users, to run the system tests
- `source.txt`: The source program we are testing against
- `sourceNumbered.txt`: A well formatted `source.txt` to be used for reference only,
  with proper indentation and line numbering
- And various test files based on relationships

### _Running all system tests at once_

1. Open a terminal in the `systemtest` folder or `cd` into it.
2. For Windows Users, run:

   ```bat
   .\serve.bat
   ```

   For Mac Users:

   Remember to set your permissions to allow for execution of your OS's respective files.

   ```bash
   chmod u+x run.sh
   ./serve.sh
   ```

   What this does is that it starts up **all** the servers needed for system tests.

   i.e If there are 6 system tests, ports 8100, 8200, 8300, ..., 8600 will be used up.

3. Everytime you change a test case and want to re run the output, run the following from the same directory:

   ```bat
   .\run.bat
   ```

   For Mac Users:

   Remember to set your permissions to allow for execution of your OS's respective files.

   ```bash
   chmod u+x run.sh
   ./run.sh
   ```

   This will trigger each individual system test's `run` file.

### _Running an inidividual system test_

E.g: To run `systemtest1`,

1.  Open a terminal in the `systemtest1` folder or `cd` into it.
2.  Copy and paste `analysis.xsl` from `Code06\tests` and paste it inside `systemtest1`.
3.  For Windows Users, run:

    ```bat
    .\run.bat
    ```

    For Mac Users:

    Remember to set your permissions to allow for execution of your OS's respective files.

    ```bash
    chmod u+x run.sh
    ./run.sh
    ```

    `.out` files should appear.

4.  To view the results of the test, run the following command in the same folder:
    (Having two terminals would help in case you want to re-run the tests)

    `python -m http.server {PORT_OF_YOUR_CHOICE}`

5.  Visit `localhost:{PORT_OF_YOUR_CHOICE}` to view the test results.

### _Todo_

- Tell which test cases are failing without going through them one by one
