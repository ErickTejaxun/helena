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
  %0 = load i32, ptr %a, align 4
  %1 = sub i32 1, %0
  ret i32 %1
}
