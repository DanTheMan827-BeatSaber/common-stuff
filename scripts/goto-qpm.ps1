function Find-QpmProjectLocation {
    param (
        [string]$startDir
    )

    <#
    .SYNOPSIS
    Finds the nearest directory containing a qpm.json file starting from a given directory.

    .DESCRIPTION
    This function searches for a qpm.json file starting from the specified directory and moving up the directory tree.
    If a qpm.json file is found, the function returns the path to the directory containing the file.
    If no qpm.json file is found, the function returns $null.

    .PARAMETER startDir
    The starting directory for the search.

    .EXAMPLE
    PS> Find-QpmProjectLocation -startDir "C:\Projects\MyProject"
    C:\Projects

    .NOTES
    This function is useful for locating QPM projects in a directory hierarchy.
    #>

    $currentDir = Get-Item -Path $startDir
    while ($null -ne $currentDir) {
        $qpmJsonPath = Join-Path -Path $currentDir.FullName -ChildPath "qpm.json"
        if (Test-Path -Path $qpmJsonPath -PathType Leaf) {
            return $currentDir.FullName
        }
        if ($null -eq $currentDir.Parent) {
            return $null
        }
        $currentDir = $currentDir.Parent
    }

    return $null
}

function Push-QpmProjectLocation {
    param (
        [string]$startDir
    )

    <#
    .SYNOPSIS
    Changes the current directory to the nearest directory containing a qpm.json file starting from a given directory.

    .DESCRIPTION
    This function searches for a qpm.json file starting from the specified directory and moving up the directory tree.
    If a qpm.json file is found, the function changes the current directory to the directory containing the file.
    If no qpm.json file is found, the function does nothing.

    .PARAMETER startDir
    The starting directory for the search.

    .EXAMPLE
    PS> GoTo-QpmProject -startDir "C:\Projects\MyProject"

    .NOTES
    This function is useful for navigating to QPM projects in a directory hierarchy.
    #>

    # Find the directory containing qpm.json
    $qpmProjectDir = Find-QpmProjectLocation -startDir "$startDir"

    if ($null -eq $qpmProjectDir) {
        Write-Error "qpm.json file not found in any parent directory."
        exit 1
    }

    Write-Output "Found QPM project at '$qpmProjectDir'"

    # Push the directory containing qpm.json to the location stack
    Push-Location -Path $qpmProjectDir
}

Push-QpmProjectLocation -startDir $PSScriptRoot
