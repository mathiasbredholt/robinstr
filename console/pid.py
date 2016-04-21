def init(pid_dict, kp, ki, kd):
    pid_dict["kp"] = kp
    pid_dict["ki"] = ki
    pid_dict["kd"] = kd
    pid_dict["i"] = 0.0
    pid_dict["d"] = 0.0
    pid_dict["error_old"] = 0.0
    pid_dict["output"] = 0.0


def update(pid_dict, target, current):
    error = target - current

    # P controller
    pid_dict["p"] = pid_dict["kp"] * error

    # I controller
    pid_dict["i"] = pid_dict["ki"] * (pid_dict["i"] + error)

    # D controller
    pid_dict["d"] = pid_dict["kd"] * (error - pid_dict["error_old"])

    pid_dict["error_old"] = error

    pid_dict["output"] = pid_dict["p"] + pid_dict["i"] + pid_dict["d"]
