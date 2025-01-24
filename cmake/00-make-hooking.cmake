include_guard()

execute_process(
    COMMAND pwsh "${CMAKE_CURRENT_LIST_DIR}/../scripts/make-hooking.ps1"
    WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/../scripts"
)
