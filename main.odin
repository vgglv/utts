package main

import "core:fmt"
import "core:os"
import "core:time"
import "core:strconv"

usage::proc() {
	fmt.println("Usage:")
	fmt.println("utts <timestamp> <time-zone (optional)>")
	fmt.println("time-zone example: gmt+9 | gmt+3")
}

main::proc() {
	if (len(os.args) < 2) {
		usage()
		return
	}
	unix_timestamp: i64 = i64(strconv.atoi(os.args[1]))
	t := time.unix(unix_timestamp, 0)
	buf_date: [time.MIN_YYYY_DATE_LEN]u8
	buf_clock: [time.MIN_HMS_LEN]u8

	formatted_date := time.to_string_dd_mm_yyyy(t, buf_date[:])
	formatted_time := time.to_string_hms(t, buf_clock[:])
	fmt.println("Formatted time:", formatted_date, formatted_time)
}