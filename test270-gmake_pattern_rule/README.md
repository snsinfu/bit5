# gmake: Confusing error message when the prerequisive of a pattern rule does not exist

The makefile in this directory tries to produce hash-%.txt from source-$.dat.
% = aaa and bbb work, but % = ccc-1 fails:

```console
$ make
sha256sum source-aaa.dat > hash-aaa.txt
sha256sum source-bbb.dat > hash-bbb.txt
make: *** No rule to make target `hash-ccc-1.txt', needed by `all'.  Stop.
```

Gmake complains that **there is no rule**, but we have a rule!

```make
hash-%.txt: source-%.dat
	sha256sum $< > $@
```

Here, indeed making hash-ccc-1.txt should not work because the corresponding
prerequisite source-ccc-1.txt does not exist. Instead, source-ccc_1.txt exists
-- this is an intentional typo. The problem is that gmake does not point out
that the prerequisite does not exist, making it hard to figure out a typo.

The behavior is not friendly but kind of understandable if I read the debug
message. Gmake actually considers the rule, but the fact that the matching
prerequisite does not exist makes gmake think that the rule is not applicable...

<details>
  <summary>Debug messages</summary>
```console
$ make -d hash-ccc-1.txt

Considering target file `hash-ccc-1.txt'.
 File `hash-ccc-1.txt' does not exist.
 Looking for an implicit rule for `hash-ccc-1.txt'.
 Trying pattern rule with stem `ccc-1'.
 Trying implicit prerequisite `source-ccc-1.dat'.
 Trying pattern rule with stem `hash-ccc-1.txt'.
 Trying implicit prerequisite `hash-ccc-1.txt,v'.
 Trying pattern rule with stem `hash-ccc-1.txt'.
 Trying implicit prerequisite `RCS/hash-ccc-1.txt,v'.
 Trying pattern rule with stem `hash-ccc-1.txt'.
 Trying implicit prerequisite `RCS/hash-ccc-1.txt'.
 Trying pattern rule with stem `hash-ccc-1.txt'.
 Trying implicit prerequisite `s.hash-ccc-1.txt'.
 Trying pattern rule with stem `hash-ccc-1.txt'.
 Trying implicit prerequisite `SCCS/s.hash-ccc-1.txt'.
 Trying pattern rule with stem `ccc-1'.
 Trying implicit prerequisite `source-ccc-1.dat'.
 Looking for a rule with intermediate file `source-ccc-1.dat'.
  Avoiding implicit rule recursion.
  Trying pattern rule with stem `source-ccc-1.dat'.
  Trying implicit prerequisite `source-ccc-1.dat,v'.
  Trying pattern rule with stem `source-ccc-1.dat'.
  Trying implicit prerequisite `RCS/source-ccc-1.dat,v'.
  Trying pattern rule with stem `source-ccc-1.dat'.
  Trying implicit prerequisite `RCS/source-ccc-1.dat'.
  Trying pattern rule with stem `source-ccc-1.dat'.
  Trying implicit prerequisite `s.source-ccc-1.dat'.
  Trying pattern rule with stem `source-ccc-1.dat'.
  Trying implicit prerequisite `SCCS/s.source-ccc-1.dat'.
 No implicit rule found for `hash-ccc-1.txt'.
 Finished prerequisites of target file `hash-ccc-1.txt'.
Must remake target `hash-ccc-1.txt'.
```
</details>
