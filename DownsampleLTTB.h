#include <cmath>

template <typename T>
inline void DownsampleLTTB(const T *data_x, const T *data_y, int data_length,
                           T *sampled_x, T *sampled_y, int threshold) {
  int sampled_index = 0;

  // Bucket size. Leave room for start and end data points
  T every = (T(data_length - 2)) / (T(threshold - 2));

  int a = 0;
  T max_area_point_x = T(0);
  T max_area_point_y = T(0);
  T max_area = T(0);
  T area = T(0);
  T next_a = T(0);

  sampled_x[sampled_index] = data_x[a];
  sampled_y[sampled_index] = data_y[a];
  sampled_index++;

  for (int i = 0; i < threshold - 2; i++) {
    // Calculate point average for next bucket (containing c)
    T avg_x = T(0);
    T avg_y = T(0);
    int avg_range_start = int(floor((T(i + 1)) * every)) + 1;
    int avg_range_end = int(floor((T(i + 2)) * every)) + 1;
    avg_range_end = avg_range_end < data_length ? avg_range_end : data_length;

    int avg_range_length = avg_range_end - avg_range_start;

    for (; avg_range_start < avg_range_end; avg_range_start++) {
      avg_x += data_x[avg_range_start];
      avg_y += data_y[avg_range_start];
    }
    avg_x /= T(avg_range_length);
    avg_y /= T(avg_range_length);

    // Get the range for this bucket
    int range_offs = int(floor((T(i + 0)) * every)) + 1;
    int range_to = int(floor((T(i + 1)) * every)) + 1;

    // Point a
    T point_a_x = data_x[a];
    T point_a_y = data_y[a];

    max_area = T(-1);
    area = T(-1);

    for (; range_offs < range_to; range_offs++) {
      // Calculate triangle area over three buckets
      area = fabs((point_a_x - avg_x) * (data_y[range_offs] - point_a_y) -
                  (point_a_x - data_x[range_offs]) * (avg_y - point_a_y)) /
             T(2);
      if (area > max_area) {
        max_area = area;
        max_area_point_x = data_x[range_offs];
        max_area_point_y = data_y[range_offs];
        next_a = range_offs;
      }
    }

    sampled_x[sampled_index] =
        max_area_point_x; // Pick this point from the bucket
    sampled_y[sampled_index] = max_area_point_y;
    sampled_index++;
    a = next_a; // This a is the next a (chosen b)
  }

  sampled_x[sampled_index] = data_x[data_length - 1];
  sampled_y[sampled_index] = data_y[data_length - 1];
}
