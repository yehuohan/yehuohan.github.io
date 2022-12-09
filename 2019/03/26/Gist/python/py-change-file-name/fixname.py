#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os

root_dir = "new"
name_prefix = "fix"

def fixname(root_dir, prefix):
    """
    Find .txt files and then rename them.
    """
    root = os.path.abspath(root_dir)
    txt_files = list(
            filter(lambda files : os.path.splitext(files)[1] == '.txt',
                map(lambda files : os.path.join(root, files),
                    os.listdir(root))))
    cnt = 1
    new_txt_files = []
    for k in range(len(txt_files)):
        new_txt_files.append(os.path.join(root, prefix + str(cnt)) + '.txt')
        os.rename(txt_files[k], new_txt_files[k])
        cnt += 1

    return [txt_files, new_txt_files]

if __name__ == "__main__":
    [txt_files, new_txt_files] = fixname(root_dir, name_prefix)

    print("Rename complete:")
    for k in range(len(txt_files)):
        print(txt_files[k] + " => " + new_txt_files[k])

    os.system('pause')
