WORKSPACE_PATH=/data2/kaplannp/
docker run --cap-add CAP_SYS_PTRACE --cap-add CAP_SYS_ADMIN --privileged -v "$WORKSPACE_PATH":/Workspace -it --name vg vg_img /bin/bash
