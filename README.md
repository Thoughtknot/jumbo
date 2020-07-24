# jumbo
Minimal database written in C.

It is tiny and non-intrusive, leaving all logic and intelligence to the user. See it as the opposite of most modern databases where sharding, datatype interpretation, keying and so on is typically left to the database. In this case, serialization, sharding, typing etc. are all left to the client allowing the database to do a very simple and contained task.

Build:
`make`

Run standalone:
`./jumbo`

This will create a `db` directory where all tables are stored. Jumbo will handle all transactions atomically, allowing for GET/PUT/DELETE operations as well as a few management operations such as GETKEYS.

Build JNI (embedded Java version):
`make jni` (Posix)

`make jni_windows` (Windows/MinGW)

Jumbo client:
There is a java client:
https://github.com/Thoughtknot/jumbo-java/

and a java JNI client:
https://github.com/Thoughtknot/jumbo-jni/
