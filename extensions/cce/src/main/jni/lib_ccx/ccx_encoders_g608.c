#include "lib_ccx.h"
#include "cce.h"
#include "ccx_encoders_common.h"
#include "ccx_encoders_helpers.h"

int write_cc_buffer_as_g608(struct eia608_screen *data, struct encoder_ctx *context)
{
	int used;
	unsigned h1, m1, s1, ms1;
	unsigned h2, m2, s2, ms2;
	int wrote_something = 0;

	millis_to_time(data->start_time, &h1, &m1, &s1, &ms1);
	millis_to_time(data->end_time - 1, &h2, &m2, &s2, &ms2); // -1 To prevent overlapping with next line.
	char timeline[128];
	context->srt_counter++;
	sprintf(timeline, "%u%s", context->srt_counter, context->encoded_crlf);
	used = encode_line(context, context->buffer, (unsigned char *)timeline);

	sprintf(timeline, "%02u:%02u:%02u,%03u --> %02u:%02u:%02u,%03u%s",
		h1, m1, s1, ms1, h2, m2, s2, ms2, context->encoded_crlf);
	used = encode_line(context, context->buffer, (unsigned char *)timeline);

	for (int i = 0; i < 15; i++)
	{
		int length = get_line_encoded(context, context->subline, i, data);

		length = get_color_encoded(context, context->subline, i, data);

		length = get_font_encoded(context, context->subline, i, data);

		wrote_something = 1;
	}

	return wrote_something;
}
