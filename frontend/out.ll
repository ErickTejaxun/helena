; ModuleID = 'Helena'
source_filename = "Helena"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 10233, ptr %a, align 4
  %b = alloca i32, align 4
  store i32 11, ptr %b, align 4
  %c = alloca i32, align 4
  store i32 12, ptr %c, align 4
  ret i32 1
}

define i32 @suma() {
entry:
  %d = alloca i32, align 4
  store i32 13, ptr %d, align 4
  ret ptr %d
}

define double @sss() {
entry:
  %e = alloca double, align 8
  store double 1.400000e+01, ptr %e, align 8
  ret ptr %e
}
