#source: pr22836-1.s
#ld: -r -s
#readelf: -g --wide
#xfail: d30v-*-* dlx-*-* i960-*-* pj-*-*
# Targets using the generic linker don't properly support comdat group sections

There are no section groups in this file\.
