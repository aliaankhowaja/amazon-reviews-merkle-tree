**Amazon Reviews Merkle Tree**

A compact C++ implementation of a Merkle tree and associated proofs (existence, integrity, tamper detection) using Amazon review data as sample input.

**Project Overview**
- **Purpose:** : Demonstrates building a Merkle tree from JSON input, generating/verifying proofs, and detecting tampering.
- **Language:** : C++ (Visual Studio solution provided).

**Highlights**
- **Solution file:** : [Merkle Tree.sln](Merkle Tree.sln)
- **Sample data:** : [data_small.json](data_small.json), [datas.json](datas.json)
- **Stored roots:** : [StoredRoots/data_small.txt](StoredRoots/data_small.txt)

**Repository Structure**
- **Core:** : [merkle_tree.cpp](merkle_tree.cpp), [merkle_tree.h](merkle_tree.h)
- **Data preprocessing:** : [data_preprocessing.cpp](data_preprocessing.cpp), [data_preprocessing.h](data_preprocessing.h)
- **Proofs & verification:** : [existence_proof.cpp](existence_proof.cpp), [existence_proof.h](existence_proof.h), [integrity_verification.cpp](integrity_verification.cpp)
- **Tamper detection & utilities:** : [tamper_detection.cpp](tamper_detection.cpp), [picosha2.h](picosha2.h)
- **Tests / Performance:** : [performance_test.cpp](performance_test.cpp)
- **Entry point:** : [main.cpp](main.cpp)

**Build (Windows / Visual Studio)**
- **Open:** : Open [Merkle Tree.sln](Merkle Tree.sln) in Visual Studio.
- **Build:** : Build the solution (Debug or Release).
- **Run:** : Run the produced executable from Visual Studio or run the built binary (for example `.\Debug\main.exe` or the path shown in your build output).

**Quick Run**
- **From Visual Studio:** : Set the startup project to the solution's executable and press F5.
- **From command line:** : After building, run the executable and provide sample JSON files located in the repository root if required by the program. Example:

```
.\Debug\main.exe
```

**Using the sample data**
- **Small sample:** : Use [data_small.json](data_small.json) for quick tests.
- **Full sample:** : Use [datas.json](datas.json) for larger runs (may take longer).
