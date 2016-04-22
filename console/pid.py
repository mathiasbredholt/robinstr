DELTA_TIME = 0.02


def init(pid_d, kp, ki, kd):
    pid_d["target"] = 0.0
    pid_d["current"] = 0.0
    pid_d["kp"] = kp
    pid_d["ki"] = ki
    pid_d["kd"] = kd
    pid_d["i"] = 0.0
    pid_d["d"] = 0.0
    pid_d["error_old"] = 0.0
    pid_d["output"] = 0.0


def update(pid_d):
    error = pid_d["target"] - pid_d["current"]

    # P controller
    pid_d["p"] = pid_d["kp"] * error

    # I controller
    pid_d["i"] += pid_d["ki"] * error * DELTA_TIME

    # D controller
    pid_d["d"] = pid_d["kd"] * \
        (error - pid_d["error_old"]) / DELTA_TIME

    pid_d["error_old"] = error

    pid_d["output"] = pid_d["p"] + \
        pid_d["i"] + pid_d["d"]
