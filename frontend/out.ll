; ModuleID = 'Helena'
source_filename = "Helena"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 111, ptr %a, align 4
  %b = alloca i32, align 4
  store i32 22, ptr %b, align 4
  %c = alloca i32, align 4
  store i32 12, ptr %c, align 4
  %0 = load ptr, ptr %c, align 8
  %1 = load i32, ptr %0, align 4
  %2 = sub i32 %1, 1
  ret i32 %2
}
