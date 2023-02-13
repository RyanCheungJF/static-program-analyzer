# System Testing

### _Folder Layout_

Each folder contains a different program and has the following files:

- `run.bat`: For Windows Users, to run the system tests
- `run.sh`: For Mac Users, to run the system tests
- `source.txt`: The source program we are testing against
- `sourceNumbered.txt`: A well formatted `source.txt` to be used for reference only,
  with proper indentation and line numbering
- And various test files based on relationships

### _Running a system test_

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

- Make it more optimized

