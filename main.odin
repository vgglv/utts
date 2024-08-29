package main

import "core:fmt"
import "core:os"
import "core:time"
import "core:strconv"

main::proc() {
	unix_timestamp: i64 = i64(strconv.atoi(os.args[1]))
	t := time.unix(unix_timestamp, 0)
	bufDate: [time.MIN_YYYY_DATE_LEN]u8
	bufClock: [time.MIN_HMS_LEN]u8

	formatted_date := time.to_string_dd_mm_yyyy(t, bufDate[:])
	formatted_time := time.to_string_hms(t, bufClock[:])
	fmt.println("Formatted time:", formatted_date, formatted_time)
}
