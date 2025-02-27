#!/usr/bin/env python3

# This Python script counts the lines of code in the directory in which it is
# run.  It only looks at files which end in the file extensions passed to the
# script as arguments.

# It outputs counts for total lines, blank lines, comment lines and code lines
# (total lines minus blank lines and comment lines).

# Example usage and output:
# > lines_of_code_counter.py .h .cpp
# Total lines:   15378
# Blank lines:   2945
# Comment lines: 1770
# Code lines:    10663

# Change this value based on the comment symbol used in your programming
# language.
commentSymbol = "//"

import sys
import os

acceptableFileExtensions = sys.argv[1:]
if not acceptableFileExtensions:
    print("Please pass at least one file extension as an argument.")
    sys.exit()

currentDir = os.getcwd()

filesToCheck = []
for root, _, files in os.walk(currentDir):
    for f in files:
        fullpath = os.path.join(root, f)
        if ".git" not in fullpath:
            for extension in acceptableFileExtensions:
                if fullpath.endswith(extension):
                    filesToCheck.append(fullpath)

if not filesToCheck:
    print("No files found.")
    sys.exit()

lineCount = 0
totalBlankLineCount = 0
totalCommentLineCount = 0

print("")
print("Filename\tlines\tblank lines\tcomment lines\tcode lines")

for fileToCheck in filesToCheck:
    with open(fileToCheck, "r", encoding="utf-8") as f:

        fileLineCount = 0
        fileBlankLineCount = 0
        fileCommentLineCount = 0

        for line in f:
            lineCount += 1
            fileLineCount += 1

            lineWithoutWhitespace = line.strip()
            if not lineWithoutWhitespace:
                totalBlankLineCount += 1
                fileBlankLineCount += 1
            elif lineWithoutWhitespace.startswith(commentSymbol):
                totalCommentLineCount += 1
                fileCommentLineCount += 1

        print(
            os.path.basename(fileToCheck)
            + "\t"
            + str(fileLineCount)
            + "\t"
            + str(fileBlankLineCount)
            + "\t"
            + str(fileCommentLineCount)
            + "\t"
            + str(fileLineCount - fileBlankLineCount - fileCommentLineCount)
        )

print("")
print("Totals")
print("--------------------")
print("Lines:         " + str(lineCount))
print("Blank lines:   " + str(totalBlankLineCount))
print("Comment lines: " + str(totalCommentLineCount))
print("Code lines:    " + str(lineCount - totalBlankLineCount - totalCommentLineCount))

a = input("")
